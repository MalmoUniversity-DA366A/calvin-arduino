#Source Code Comments

Doxygen can automate the documenting export process from source code to a Markdown document. To make this possible the comments must follow Javadoc standard (http://www.stack.nl/~dimitri/doxygen/manual/docblocks.html). Following is an example of how the comments should be made above a function:

/**

* A sort of headline describing the following function

* @param a description of the parameters that are passed to the function

* @return a description of the return value from the function

*/

Source code to MarkDown

Doxygen can be set to include all functions, even those without comments. This is done by choosing Mode -> all entities. Doxygen ignores comments done with // some comment here, and /* some comment here */. A plain HTML (.html) document is constructed and the completed document is then converted to Markdown (.md) with a program called PanDoc (https://github.com/jgm/pandoc/releases/tag/1.15.0.6). PanDoc uses cmd/terminal code. To convert from .html to .md, write:

pandoc -f html -t markdown_strict filenameToReadFrom.html -s -o fileNameToWriteTo.md

Word to MarkDown

Pandoc supports a wide variety of formats and can even be used to convert a Word file to Markdown. To convert from .docx to .md, write:

pandoc -f docx -t markdown filenameToReadFrom.docx -s -o fileNameToWriteTo.md

**Source code comments**

Doxygen can automate the documenting export process from source code to
a Markdown document. To make this possible the comments must follow
Javadoc standard
(<http://www.stack.nl/~dimitri/doxygen/manual/docblocks.html>).
Following is an example of how the comments should be made above a
function:

/\*\*

\* *A sort of headline describing the following function*

\* *@param a description of the parameters that are passed to the
function*

\* *@return a description of the return value from the function*

\*/

**Source code to MarkDown**

Doxygen can be set to include all functions, even those without
comments. This is done by choosing *Mode -&gt; all entities*. Doxygen
ignores comments done with // some comment here, and /\* some comment
here \*/. A plain HTML (.html) document is constructed and the completed
document is then converted to Markdown (.md) with a program called
PanDoc (<https://github.com/jgm/pandoc/releases/tag/1.15.0.6)>. PanDoc
uses cmd/terminal code. To convert from .html to .md, write:

*pandoc -f html -t markdown\_strict filenameToReadFrom.html -s -o
fileNameToWriteTo.md*

**Word to MarkDown**

Pandoc supports a wide variety of formats and can even be used to
convert a Word file to Markdown. To convert from .docx to .md, write:

*pandoc -f docx -t markdown filenameToReadFrom.docx -s -o
fileNameToWriteTo.md*
