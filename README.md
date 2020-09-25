# TeXhelper

Various helpful scripts for typesetting TeX articles.

1. bibSort. Sorts references in the bibliography list in the same order in which they appear in the paper. No more need to manually keep track of which reference is mentioned before which other reference.

To use the program please input the name of the main .tex file (which contains the bibliography list) in the argument of

bibSort bs("name.tex");

located on line 196.

Place the .tex file into the same directory as the bibSort.cpp

Compile, e.g.

g++ -std=c++17 -o bibSort bibSort.cpp

Execute:

./bibSort

Your references will be available as an output in the console. Please copy them and paste them into the .tex file as your bibliography list.

The output will ignore references included in the original bibliography but not cited in the main text.

Please ensure that the references in the text are separated with a comma, not a dash.

I will work on a more user-friendly interface if the conditions of a sufficient demand on the part of users and sufficient time on my part are both met.
