import sys
import json
import re


def main():
    twitter_file = open(sys.argv[1])

    tweets = []
    for line in twitter_file:
        l = json.loads(line)
        if 'text' in l:
            tweets.append(l['text'].split(' '))

    freq = {}
    total = 0
    for tweet in tweets:
        for word in tweet:
            total += 1
            y = re.sub(r'\W+', '', word.lower().encode('ascii', 'ignore'))

            if y not in freq:
                freq[y] = 0
            freq[y] += 1

    for y in freq:
        freq[y] = float(freq[y])/float(total)

    for word, score in freq.iteritems():
        print word + " " + str(score)


if __name__ == '__main__':
    main()
