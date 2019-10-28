FILENAME = 'Stack.h'
file = open('./' + FILENAME, 'r')
text = file.read()
file.close()


one_word = ''
for l in text:
    one_word += l
one_word = list(one_word.split('\n'))
one_word = '#'.join(one_word)
one_word = one_word.split('#')



words = []
for word in one_word:
    if (len(word) > 0):
        words += word.split(' ')

words_ = []
for l in words:
    if len(l) > 0:
        words_.append(l)

ok = [''] * len(words_)
for i in range(len(words_)):
    for j in range(len(words_[i])):
        if (words_[i][j].isalpha() or (words_[i][j].isdigit() and j > 0)):
            ok[i] += words_[i][j]

words_ = ok
clear = []
for i in words_:
    if (len(i) > 0 and not(i[0].isdigit())):
        clear.append(i)
words_= clear
words_with_undef = []
for l in words_:
    words_with_undef.append('#undef ' + l)

file = open('./' + FILENAME, 'w')
for w in words_with_undef:
    file.write(w + "\n")
for w in text:
    file.write(w)
file.close() 
