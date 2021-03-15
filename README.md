# ww
Word Wrapping Program in C

Partner 1: Norman Ligums Clark
RUID: 153004988

Partner 2: Hasin Choudhury
RUID: 185008632

Initially testing was accomplished using visual inspection of standard output from sample text. These results were expanded by including sample text containing various special cases such as beginning with a \n and containing a word bigger than the specified wrap width among others. More complete testing was accomplished via running large amounts of sample text from wikipedia and Lorem Ipsum text generators through our program and using the ‘wc’ command on the input and the output. Because POSIX states “the wc utility shall consider a word to be a non-zero-length string of characters delimited by white space” which is consistent with the definition of a word for our program, we expected identical word counts after execution. Additionally, we knew that once a file was wrapped to a specific width, if we ran our program with the same width again on the output the results should be identical if our program was working properly. This was accomplished quickly by redirecting the output to a test file and then running the program again on the test file and comparing the results with ‘cmp’. To be more confident in the exact results on large testing files we created a testing program which compared the number of characters in between newlines to the input width and looked for the presence of spaces in front of every newline. Both of these conditions should be false if our program was working properly. Wrapping to a specified width inherently means at most the number of characters in between newlines should be the size of the width (assuming no words were longer than the width). When wrapping the directions indicated that words should have at most 1 whitespace character in between them thus the presence of a space next to a newline would indicate that the output contained two whitespace characters in between words and a distortion of the text according to the assignment parameters. Our testing program would indicate if either of these conditions were true which would indicate an issue with the underlying program.

Exit status was checked via the ‘echo $?’ command after running the program on test cases in which we knew if the longest word exceeded the width length. Additional functionality was tested using manual input into standard input, redirecting files into standard input, inputting absolute and relative directory paths, inputting empty text files and directories, changing the size of our buffer, and inputting invalid file names.

Running wordcounts, comparisons, and our testing program on the output of a multitude of different inputs gives us confidence in our programs' functionality.
