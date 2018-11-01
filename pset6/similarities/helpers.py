from nltk.tokenize import sent_tokenize


def lines(a, b):
    """Return lines in both a and b"""

    # Using set to remove duplicates (lines that are in common)
    # Splitting each strings into individual lines
    lines_a = set(a.split("\n"))
    lines_b = set(b.split("\n"))

    # Return list containing all the lines present in string a and string b
    return lines_a & lines_b


def sentences(a, b):
    """Return sentences in both a and b"""

    # Using set to remove duplicates (sentences that are in common)
    # Split each strings into sentences using sent_tokenize
    sentences_a = set(sent_tokenize(a))
    sentences_b = set(sent_tokenize(b))

    # Return list containing all the sentences that appear in both original strings
    return sentences_a & sentences_b

# Helper function to take a string and obtain substring of length n
def substring_tokenize(str, n):
    # Generating an empty list
    substrings = []

    # Iterate over length of string
    for i in range(len(str) - n + 1):
        # Append string of length i to i + n to list
        substrings.append(str[i:i+n])

    return substrings


def substrings(a, b, n):
    """Return substrings of length n in both a and b"""

    substrings_a = set(substring_tokenize(a,n))
    substrings_b = set(substring_tokenize(b,n))

    # Return list containing all substrings that appear in both files
    return substrings_a & substrings_b