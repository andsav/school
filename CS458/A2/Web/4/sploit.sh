#!/bin/bash

POST_URL="${1}/post.php"
VOTE_URL="${1}/vote.php"
CONFIRM_URL="${1}/confirm.php"

if [ ! -f data.db ]; then
    echo "Fetching database file"
    wget --quiet "$1/docs/data.db"
fi

SQL=$'SELECT username, confirm FROM users WHERE confirm <> \'\';'
echo "$SQL" | sqlite3 data.db | while read line; do
    credentials=(${line//|/ })
    username="${credentials[0]}"
    confirm="${credentials[1]}"

    echo "User ${username} is unconfirmed. Attempting to log in with confirmation code ${confirm}"
    curl -L -c cookies -s "${CONFIRM_URL}?hash=${confirm}" | awk '/Welcome, ([a-z]+)!/ { print $1 " " $2 }'
    
    echo "Attempting downvotes..."
    for i in {1..12}; do
        curl -L -b cookies -s "${VOTE_URL}?id=${i}&vote=-1" > /dev/null
    done
done

rm data.db cookies
