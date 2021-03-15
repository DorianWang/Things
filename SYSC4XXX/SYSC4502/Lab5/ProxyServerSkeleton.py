from socket import *
import sys
import traceback

if len(sys.argv) <= 1:
	print('Usage : "python ProxyServer.py server_ip"\n[server_ip : It is the IP Address Of Proxy Server')
	sys.exit(2)
	
# Create a server socket, bind it to a port and start listening
tcpSerSock = socket(AF_INET, SOCK_STREAM)
# Fill in start
tcpSerSock.bind((sys.argv[1], 8888))
tcpSerSock.listen()
# Fill in end

while 1:
	# Start receiving data from the client
	print('Ready to serve...')
	tcpCliSock, addr = tcpSerSock.accept()
	print('Received a connection from:', addr)
	message = str(tcpCliSock.recv(1024))  # Fill in here
	print(message)
	# suppress processing of requests for favicon
	if message.split()[1] == "/favicon.ico":
		print("Suppress request for favicon")
		continue
	filename = message.split()[1].partition('/')[2]

	print(filename)
	fileExist = "false"
	filetouse = "/" + filename.replace("/", "-")
	print("File to use " + filetouse)
	try:
		# Check whether the file exists in the cache
		f = open(filetouse[1:], "r")                      
		outputdata = f.readlines()                        
		fileExist = "true"
		print("Service file from cache")
		# ProxyServer finds a cache hit and generates a response message
		tcpCliSock.send("HTTP/1.0 200 OK\r\n".encode())            
		tcpCliSock.send("Content-Type:text/html\r\n\r\n".encode())

		# Fill in start
		for line in outputdata:
			tcpCliSock.send(line.encode())
			tcpCliSock.send("\r\n".encode())
		tcpCliSock.send("\r\n".encode())
		f.close()
		# Fill in end

	# Error handling for file not found in cache
	except IOError:
		if fileExist == "false": 
			# Create a socket on the proxyserver
			c = socket(AF_INET, SOCK_STREAM)  # Fill in here
			hostn = filename.split("/")[0].replace("www.", "", 1)
			print("Host " + hostn)                                   
			try:
				# Connect to the socket to port 80
				print(str(hostn))
				c.connect((str(hostn), 80))  # Fill in here
				message = "GET "+"http://" + filename + " HTTP/1.0\r\n\r\n"
				c.send(message.encode())  
				# Read the response into buffer
				
				# Fill in start
				message_data = list()
				buffer = str()
				running = True
				while running:
					buffer += str(c.recv(1024))
					print(buffer)
					chunks = buffer.split('\\r\\n')
					buffer = chunks.pop()  # last value of buffer is kept, as it may be part of the next line.
					message_data.extend(chunks)
					print(message_data)
					for item in chunks:
						if item.startswith('</html>'):
							running = False
							break
				content_start = False
				page_content = []

				for line in message_data:
					if content_start:
						page_content.append(line)
					else:
						if line.startswith('<html>'):
							page_content.append(line)
							content_start = True
				c.close()
				# Fill in end

				# Create a new file in the cache for the requested file.
				# Also send the response in the buffer to client socket and the corresponding file in the cache
				tmpFile = open("./" + filetouse, "w")

				# Fill in start
				if message_data[0].find('404'):
					tcpCliSock.send("HTTP/1.0 404 File Not Found\r\n".encode())
					tcpCliSock.send("Content-Type:text/html\r\n\r\n".encode())
					for line in page_content:
						tcpCliSock.send(line.encode())
						tcpCliSock.send("\r\n".encode())
					tcpCliSock.send("\r\n".encode())
					tmpFile.close()
					continue
				tmpFile.writelines(page_content)
				tmpFile.close()
				tcpCliSock.send("HTTP/1.0 200 OK\r\n".encode())
				tcpCliSock.send("Content-Type:text/html\r\n\r\n".encode())
				for line in page_content:
					tcpCliSock.send(line.encode())
					tcpCliSock.send("\r\n".encode())
				tcpCliSock.send("\r\n".encode())
				# Fill in end
			except Exception as e:
				print(e)
				print("Illegal request")
				traceback.print_exc()                                              
		else:
			# HTTP response message for file not found
			
			# Fill in start
			tcpCliSock.send("HTTP/1.0 404 Not Found\r\nContent-Length:22; \
								Content-Type:text/plain; charset=utf-8\r\n\r\n".encode())
			# Fill in end
	
	# Close the client and the server sockets    
	tcpCliSock.close() 

# Fill in start
tcpSerSock.close()

# Fill in end
