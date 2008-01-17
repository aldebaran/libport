#! /bin/sh

openssl req -newkey rsa:1024 -sha1 -keyout rootkey.pem -out rootreq.pem

openssl x509 -req -in rootreq.pem -sha1 -extensions certificate_extensions -signkey rootkey.pem -out rootcert.pem

cat rootcert.pem rootkey.pem > ca.pem



openssl req -newkey rsa:1024 -sha1 -keyout serverCAkey.pem -out serverCAreq.pem

openssl x509 -req -in serverCAreq.pem -sha1 -extensions certificate_extensions -CA ca.pem -CAkey ca.pem -CAcreateserial -out serverCAcert.pem

cat serverCAcert.pem serverCAkey.pem rootcert.pem > serverCA.pem



openssl req -newkey rsa:1024 -sha1 -keyout serverkey.pem -out serverreq.pem

openssl x509 -req -in serverreq.pem -sha1 -extensions certificate_extensions -CA serverCA.pem -CAkey serverCA.pem -CAcreateserial -out servercert.pem

cat servercert.pem serverkey.pem serverCAcert.pem rootcert.pem > server.pem


openssl req -newkey rsa:1024 -sha1 -keyout clientkey.pem -out clientreq.pem

openssl x509 -req -in clientreq.pem -sha1  -extensions certificate_extensions -CA ca.pem -CAkey ca.pem -CAcreateserial -out clientcert.pem

cat clientcert.pem clientkey.pem rootcert.pem > client.pem

openssl dhparam -check -text -5 512 -out dh512.pem

