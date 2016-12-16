install.packages(c('repr', 'IRdisplay', 'crayon', 'pbdZMQ', 'devtools'))
devtools::install_github('IRkernel/IRkernel')
IRkernel::installspec() 
IRkernel::installspec(name = 'ir33', displayname = 'R 3.3')

