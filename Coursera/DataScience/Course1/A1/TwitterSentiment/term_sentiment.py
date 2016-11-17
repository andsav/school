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

    words_score = {}
    for tweet in tweets:
        a = 0.0
        counted_words = 1
        absent_words = []

        for word in tweet:
            y = re.sub(r'\W+', '', word.lower().encode('ascii', 'ignore'))

            if y in scores:
                a += float(scores[y])
            elif y in words_score:
                a += words_score[y]
            else:
                absent_words.append(y)

            counted_words += 1

        for word in absent_words:
            if word not in words_score:
                words_score[word] = 0.0

            words_score[word] += a/counted_words

    for word, score in words_score.iteritems():
        print word + " " + str(score)


if __name__ == '__main__':
    main()
