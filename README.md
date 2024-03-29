# Regex

This is a Regex Model. It has the 4 basic regex operators with added functionality to enhance utility and practicality. Its salient aspects include a minimal use of reserved characters, whitespace insensitivity( **\\s** is how you write a space), and the ability to manipulate and declare custom character sets that make the corresponding matching process more efficient. Lastly, the regex scheme has overlap with many pre-existing regex schemes. Especially in its use of '.' as a universal character representation, and '-' to declare ranges of characters.

A proper specification of the scheme is as follows: 

# Regex mode
**This is the default mode of processing the regex. It works as most schemes work. '\*' is kleene closure, '+' is conventional closure, '?' is an optional marker, and '|' is alternation. '(' , ')' are used to group subexpressions.**

eg:

**(ab)+**
    matches all sequences of the characters 'ab' that repeat at least once.

**ab+**
    matches all sequences starting with an 'a' followed by one or more 'b' characters.

**ab?**
    matches 'a' or 'ab' - the 'b' is optional.
Another example is **colou?r**, which matches 'color' or 'colour'

**ab\*|a\*b**
    matches the sequences determined by **ab\*** or **a\*b**.
A real world example would be **me(t|d)al**, which matches 'metal' or 'medal'.

*So far we have only covered a basic regex specification, but the specification is extended to specify custom character sets*

# Character Set mode: 
**This is enclosed between a '\[' and '\]'**

we used something like this before   **me\[t|d\]al**, except the () are replaced by \[\].

This matches 'metal' and 'medal' just like alternation, but it does it by using a set of characters as opposed to creating alternate paths through the regex. This is useful as it makes the matching process faster. It can also make the regexes more readable. For example,

**\[A-Z\]**    specifies the set of uppercase letters. Without this, we'd need to use this:    **(A|B|D|E|F|G|H|I...|Z)** vs which is much more difficult to read and for the computer to evaluate.

Within charset mode, you can manipulate these sets using set operations. Here is a list of the operators:

**'~'**    takes a char set and returns everything not contained within it.

eg:

**\[~s\]**    includes every character in the ASCII scheme that is not an 's'.

**\[~A-Z\]**    includes every character that is not an uppercase letter.

**\[~0-9\]**    includes every character that is not a digit.

**'&'**    takes two character sets and returns their intersection

eg:

**\[~A-D & A-Z\]**    takes all the uppercase letters except for A,B,C,D. Since we intersect whatever isn't contained in A-D with A-Z. This effectively removes the characters {A, B, C, D} from the set of uppercase letters.

**'|'** takes 2 charsets and returns their union:

eg:

**\[A-Z|0-9\]**    This charset contains all uppercase letters and all digits.

Declaring basic charsets - 3 ways:

1) The character itself.

**\[a|b\]**    a defines the union of the charset containing 'a', with the charset containing 'b'.

2) A range of characters:

**\[a-d\]**    defines all the ascii characters between 'a' and 'd' inclusive. This results in {a,b,c,d}, but this might vary, given the structure of Ascii.

3) Use predefined macros:

**\#D**    represents digits, **\#A** represents alphabetical characters, **\#!** represents all punctuation characters, etc.

E.g.

**\#A\[\#A|\#D\]\***    This matches a letter, followed by 0 or more characters that are either letters or numbers.


A list of all the predefined character sets are shown below, these are preceded by a **\#** when called in the regex. The letters used to specify the charset are case insensitive. So for specifying the set of all letters, **\#A** ans **\#a** both work.

**S**:    {space, tab, line feed, carraige return, vertical tab, form feed} All whitespace characters

**U**:    \[A-Z\]

**L**:    \[a-z\]

**A**:    \[#U|#L\]

**D**:    \[0-9\]

**W**:    \[#A|#D|\_\]

**X**:    \[#D|a-f|A-F\]

**P**:     \!, \", \#, \$, \%, \&, ', (, ), \*, +, -, \., \/, :, ;, ?, @, \[, \\, \], ^, \_, \`, {, |, }, ~  and the comma itself.  note that '<=>' are not present

**!**:    an alias for P

**E**:    all characters

**.**:    an alias for E

**Z**:    no characters

**0**:    an alias for Z

**Real World Examples:**

**\[\#A|\_\]\#W\*** 

The above regex matches all valid variable names in C. A letter or underscore followed by either letters, numbers, underscores or nothing.

**(+1)?\#D\#D\#D-?\#D\#D\#D-?\#D\#D\#D\#D?** 

The above expression matches all phone numbers with or without dashes in between groups of 3 or 4 digits, and it allows an optional US country code if provided.

**(\[1-9\]|(1\[0-2\]))/(\[1-9\]|\[1|2\]#D|3\[0|1\])(/(\#D\#D)?\#D\#D)?**

The above regex matches all valid mm/dd/yyyy combinations with an optional year and support for 2 digit years as well. However, 2/30/2022 is still accepted even though that date isn't valid in reality.

**\#W+**

This matches a word, which is a set of one or more alphanumeric characters.

# A few extra details: 

Any and all reserved characters can be escaped using \c where 'c' is the character to be escaped. \\ escapes itself. A few reserved sequences are \ followed by {n, t, s, v, f, r} which correspond to {line feed, tab, space, vertical tab, form feed, carraige return}

This scheme only matches regular languages. It does not have a way to compute backreferences or backtrack through the regex.
