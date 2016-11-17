import sys
import json
import re
import operator

def main():
    twitter_file = open(sys.argv[1])

    hashtags = {}
    for line in twitter_file:
        l = json.loads(line)
        if 'entities' in l:
            if 'hashtags' in l['entities']:
                for t in l['entities']['hashtags']:
                    if t['text'] not in hashtags:
                        hashtags[t['text']] = 0
                    hashtags[t['text']] += 1

    sorted_hashtags = sorted(hashtags.items(), key=operator.itemgetter(1), reverse=True)

    for i in range(0, 10):
        print sorted_hashtags[i][0] + " " + str(sorted_hashtags[i][1])

if __name__ == '__main__':
    main()
