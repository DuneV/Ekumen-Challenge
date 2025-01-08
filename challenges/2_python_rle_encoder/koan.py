#!/bin/env python

# Run Length Encoding (RLE) is a simple form of data compression, where runs
# (consecutive data elements) are replaced by just one data value and count.
# For example we can represent the original 53 characters with only 13.
# "WWWWWWWWWWWWBWWWWWWWWWWWWBBBWW" -> "12W1B12W3B2W"

# Implement a simple class with two methods, encode and decode, to perform
# Run Length Encoding and Decoding. on a string.


import unittest

# BEGIN EDIT ------------------------------------------------------


class RLEEncoder:

    def encode(self, data)->str:
        '''
        
        parameters: (str) the initial string value (photo encrypted SHA-256 or others)
        fun: count the quantity of values and represent the value
        return: (str) the string enconde
        
        '''
        try:
            if not data:
                return ""
            # new value
            encoded = []
            # quantity of repetitions
            counter = 1
            
            for i in range(1, len(data)):
                if data[i] == data[i-1]: # it makes sure if the actual is equal to the previus
                    counter += 1
                else:
                    # allocated the quantity and the value
                    encoded.append(f"{counter}{data[i-1]}")
                    # reset the counter
                    counter = 1
            
            encoded.append(f"{counter}{data[-1]}")
            
            return "".join(encoded)
        
        except Exception as errno:
            print("The fail was produced by: " + errno)

    def decode(self, data)->str:
        '''
        parameters: (str) the final string decoded
        fun: count the values of the characters and multiply them.
        return: (str) the string decode
        
        '''
        try:
            if not data:
                return ""
            
            decoded = []
            counter = ""
            
            for char in data:
                if char.isdigit(): # condition of evaluation
                    counter += char
                else:
                    decoded.append(char * int(counter))
                    counter = ""
            return "".join(decoded)
        except Exception as errno:
            print("The fail was produced by: " + errno)

# END EDIT --------------------------------------------------------


class RLETest(unittest.TestCase):

    def test_trivial_encoding(self):
        encoder = RLEEncoder()
        self.assertEqual(encoder.encode("A"), "1A")
        self.assertEqual(encoder.encode("BB"), "2B")
        self.assertEqual(encoder.encode("ACBBCCC"), "1A1C2B3C")
        self.assertEqual(
            encoder.encode("WWWWWWWWWWWWBWWWWWWWWWWWWBBBWW"), "12W1B12W3B2W"
        )
        self.assertEqual(encoder.encode(""), "")

    def test_trivial_decoding(self):
        encoder = RLEEncoder()
        self.assertEqual(encoder.decode("1A"), "A")
        self.assertEqual(encoder.decode("2B"), "BB")
        self.assertEqual(encoder.decode("1A1C2B3C"), "ACBBCCC")
        self.assertEqual(
            encoder.decode("12W1B12W3B2W"), "WWWWWWWWWWWWBWWWWWWWWWWWWBBBWW"
        )
        self.assertEqual(encoder.decode(""), "")

    def test_encoding_and_decoding(self):
        encoder = RLEEncoder()
        self.assertEqual(encoder.decode(encoder.encode("ACBBCCC")), "ACBBCCC")
        self.assertEqual(
            encoder.decode(
                encoder.encode("WWWWACBBCCCWWWWWWWWBWWWWWHFFFSDSSFDGWWWWWWWBBBWW")
            ),
            "WWWWACBBCCCWWWWWWWWBWWWWWHFFFSDSSFDGWWWWWWWBBBWW",
        )
        self.assertEqual(encoder.decode(encoder.encode("")), "")


if __name__ == "__main__":
    unittest.main()
