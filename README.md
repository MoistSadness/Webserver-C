## A simple Webserver

For the past year, I have been focusing my learning time on Web Development, so I figured building a webserver in C would be a fantastic opportunity to learn about how webservers actually work on top of being a cool flex on other web developers.

The goal will be to have this eventually deployed on AWS and accessible to the wider internet. However, I will be putting this on hold for now to focus on other projects since the base application is complete

## Now that the basic webserver has been created, there are are couple of things that can be added to make it more functional
- :white_check_mark: Serve HTML files instead of hard coded strings
    - :white_check_mark: Style the pages with Bootstrap to make things super easy on the frontend
    - :white_check_mark: Add QOL features like boilerplate and templates if it doesn't make things too complicated
- Improve the logging system (ie. add timestamps to incoming requests)
- Add command line arguements so user can decide what port to run on at runtime
- :white_check_mark: Run some code based on the method. Set up a switch statement to determine what do if the request is GET, POST, etc... 
- Route the reqest based on the URI. Use a filesystem based routing system like in Next.js
- Thread the server to make it able to handle concurrent 
- Maybe add some DB functionality 
- Unit testing

### Resources:
- https://bruinsslot.jp/post/simple-http-webserver-in-c/
- https://github.com/bloominstituteoftechnology/C-Web-Server/blob/master/src/server.c