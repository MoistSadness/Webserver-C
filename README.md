## A simple Webserver

For the past year, I have been focusing my learning time on Web Development, so I figured building a webserver in C would be a fantastic opportunity to learn about how webservers actually work on top of being a cool flex on other web developers.

The goal will be to have this eventually deployed on AWS and accessible to the wider internet. 


## Now that the basic webserver has been created, there are are couple of things that can be added to make it more functional
- Serve HTML files instead of hard coded strings
    - Style the pages with Bootstrap to make things super easy on the frontend
    - Add QOL features like boilerplate and templates if it doesn't make things too complicated
- Improve the logging system (ie. add timestamps to incoming requests)
- Add command line arguements so user can decide what port to run on at runtime
- Run some code based on the method. Set up a switch statement to determine what do if the request is GET, POST, etc... 
- Route the reqest based on the URI. Use a filesystem based routing system like in Next.js
- Thread the server to make it able to handle concurrent 
- Maybe add some DB functionality 
- Unit testing

### Resources:
- https://bruinsslot.jp/post/simple-http-webserver-in-c/