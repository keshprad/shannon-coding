class ShannonCoder:
    def __init__(self, syms):
        self.syms = syms

        # setup frequencies
        self.freqs = self.calculate_frequencies()
        self.freqs.sort(key=lambda k: k[1], reverse=True)

        # assign codes to each symbol
        self.codes = {}
        self.assign_codes(0, len(self.freqs)-1, "")

        # create trie for decoding above codes
        self.decode_trie = self.build_decode_trie()

    def calculate_frequencies(self):
        freq = {}
        for sym in self.syms:
            freq[sym] = freq.get(sym, 0) + 1
        return list(freq.items())

    def assign_codes(self, l, r, code):
        if l == r:
            self.codes[self.freqs[l][0]] = code
            return

        # determine index to split at
        total = sum([self.freqs[i][1] for i in range(l, r+1)])
        i, acc = l, 0
        while acc <= total // 2:
            acc += self.freqs[i][1]
            i += 1
        # check whether left half or right half should be bigger
        prevacc = acc - self.freqs[i-1][1]
        if abs(acc - (total - acc)) > abs(prevacc - (total - prevacc)):
            acc = prevacc
            i -= 1

        # recurse on left
        self.assign_codes(l, i-1, code+"0")

        # recurse on right
        self.assign_codes(i, r, code+"1")

    def build_decode_trie(self):
        trie = {}

        # add every symbol to trie
        for sym in self.codes:
            curr = trie
            code = self.codes[sym]

            # traverse trie to leaf
            for i in range(len(code)-1):
                curr[code[i]] = curr.get(code[i], {})
                curr = curr[code[i]]
            # set leaf to symbol
            # no need for terminal char ($) since no code will be a prefix of another
            curr[code[-1]] = sym
        return trie

    def encode_text(self):
        text = [self.codes[sym] for sym in self.syms]
        return ''.join(text)

    def decode_text(self, enc):
        res = ""
        curr = self.decode_trie
        for i in range(len(enc)):
            if isinstance(curr[enc[i]], str):
                # at leaf
                # decode to symbol
                res += curr[enc[i]]
                # start at top of trie for next symbol encoding
                curr = self.decode_trie
            else:
                # traverse trie
                curr = curr[enc[i]]
        return res


if __name__ == "__main__":
    sc = ShannonCoder("Happy India Independence Day!!")
    enc = sc.encode_text()
    print(enc)
    dec = sc.decode_text(enc)
    print(dec)
