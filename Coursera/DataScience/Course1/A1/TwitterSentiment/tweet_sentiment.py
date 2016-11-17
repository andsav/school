import sys
import json
import re


def main():
    afinnfile = open(sys.argv[1])
    twitter_file = open(sys.argv[2])

    scores = {}
    for line in afinnfile:
        term, score = line.split("\t")
        scores[term] = int(score)

    tweets = []
    for line in twitter_file:
        l = json.loads(line)
        if 'text' in l:
            tweets.append(l['text'].split(' '))

    for tweet in tweets:
        a = 0
        for word in tweet:
            y = re.sub(r'\W+', '', word.lower().encode('ascii', 'ignore'))
            a += scores.get(y, 0)
        print a

if __name__ == '__main__':
    main()
