## There are five steps to creating a webserver
1. Create a socket
2. Bind socket to a port
3. Listen on the port
4. Accept requests from the internet
5. Perform required actions

## File IO has always been something I have had issues with when programming in C. This was a pretty good oportunity to sit down and get a bit more practice in, this time with practical applications
- https://stackoverflow.com/questions/53054089/reading-content-from-a-file-and-storing-it-to-string-in-c


There was a situation where the compiler was telling me I was having memory leaks due to the character pointer I malloc'ed already being freed

---

my server was only sending a fraction of the response. When I took a deeper dive, it turned out that I was allocting 393 bytes of memory, but was only sending 

Making boilerplate html is difficult
Especially with the stuff inside the <html> tag

Given the following code
<html>
    { Code from a different file here }
</html>

The program will have to parse the webpage until it reaches the '{' character, inject the code from the other file, skip the placeholder code, and then continue on it's way. That wouldn't be too hard if I were using a higher level language like Python that have easier ways of dealing with strings, but given that I have to allocate memory and keep track of it's size, it becomes much more complicated.







