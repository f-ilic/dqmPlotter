import httplib, socket, ssl, os
from httplib import HTTPConnection, HTTPS_PORT
from urlparse import urlparse
from HTMLParser import HTMLParser  
from ROOT import *

#############################################################################
###           HELPER
#############################################################################

class HTTPSConnection(HTTPConnection):
  "This class allows communication via SSL."
  default_port = HTTPS_PORT

  def __init__(self, host, port=None, key_file=None, cert_file=None,
              strict=None, timeout=socket._GLOBAL_DEFAULT_TIMEOUT,
              source_address=None):
    HTTPConnection.__init__(self, host, port, strict, timeout, source_address)
    self.key_file = key_file
    self.cert_file = cert_file

  def connect(self):
    "Connect to a host on a given (SSL) port."
    sock = socket.create_connection((self.host, self.port),
                                    self.timeout, self.source_address)
    if self._tunnel_host:
      self.sock = sock
      self._tunnel()
    # this is the only line we modified from the httplib.py file
    # we added the ssl_version variable
    self.sock = ssl.wrap_socket(sock, self.key_file, self.cert_file, ssl_version=ssl.PROTOCOL_TLSv1)

#now we override the one in httplib
httplib.HTTPSConnection = HTTPSConnection

#############################################################################
###           CRAWLER IMPLEMENTATION
#############################################################################

# We are going to create a class called LinkParser that inherits some
# methods from HTMLParser which is why it is passed into the definition
class LinkParser(HTMLParser):

  # This is a function that HTMLParser normally has
  # but we are adding some functionality to it
  def handle_starttag(self, tag, attrs):
    if tag == 'a':
      for (key, value) in attrs:
        if key == 'href':
          # print(value)
          
          if value.endswith("root"):
            self.files.append(value)
          elif len(value.split("/")) > self.relURLDepth:
            self.links.append(value)

  # This is a new function that we are creating to get links
  # that our spider() function will call
  def getLinks(self, hostURL, relURL, key_file = None, cert_file = None):
    self.links = []
    self.files = []
    
    self.relURLDepth = len(relURL.split("/"))
    
    self.baseUrl = hostURL
    URL = urlparse(hostURL + relURL)
    connection = httplib.HTTPSConnection(URL.hostname, 
                                        key_file=key_file, 
                                        cert_file=cert_file)
    connection.request('POST', URL.path + URL.query)
    response = connection.getresponse()
    
    print("Visiting: %s, %d" % (hostURL + relURL, response.status))
    # print(response.read())
    
    htmlBytes = response.read()
    htmlString = htmlBytes.decode("utf-8")
    self.feed(htmlString)
    return htmlString, self.links, self.files

def spider(hostURL, relURL, key_file, cert_file):  
  pagesToVisit = [relURL]
  listOfFiles = []
  # The main loop. Create a LinkParser and get all the links on the page.
  while pagesToVisit != []:
    # Start from the beginning of our collection of pages to visit:
    url = pagesToVisit[0]
    pagesToVisit = pagesToVisit[1:]
    # try:
    parser = LinkParser()
    data, links, files = parser.getLinks(hostURL, url, key_file, cert_file)
    
    pagesToVisit = links + pagesToVisit
    listOfFiles = listOfFiles + files
    # except:
      # print(" **Failed!**", url)
      
  # for file in listOfFiles:
    # print(hostURL + file)
    
  return listOfFiles;

##################################################################################

def getMaxRun():
  os.system("python utils/rhapi.py \"select max(r.runnumber) as runMax from runreg_tracker.runs r where r.runnumber > 300000\" --all -f json > tmp.tmp")
  with open("tmp.tmp", "r") as commandResult:
    content = ''.join(commandResult.readlines())
    maxRun = int(content.strip()[1:-1].split(":")[1].strip()[2:-2])
    print("Max run: %d" %(maxRun))
    
    return maxRun
    
#1 STARTDIRs PATH or UPDATE
#2 PATHTODB
#3 PATHTOGLOBUS

hostURL = "https://cmsweb.cern.ch"
startDirs = ["/dqm/online/data/browse/Original/"]
pathToDb = "rootFiles.db"
pathToGlobus = "/afs/cern.ch/user/p/pjurgiel/.globus/copy/"

print(os.sys.argv)
for i in range(1, len(os.sys.argv)):
  if i == 1:
    if os.sys.argv[i] == "UPDATE":
      currMax = getMaxRun()
      runHighStr = str(currMax // 10000)
      leadingZeros = 5 - len(runHighStr)
      startDirs = ["/dqm/online/data/browse/Original/" + ("0" * leadingZeros) + runHighStr + "xxxx/"]
    elif os.sys.argv[i] == "default":
      continue    
    else:
      with open(os.sys.argv[i], "r") as pathFile:
        startDirs = pathFile.readlines()
        startDirs = [line.strip() for line in startDirs]
  elif i == 2:
    pathToDb = os.sys.argv[i]
  elif i == 3:
    pathToGlobus = os.sys.argv[i]

key_file = pathToGlobus + "userkey_nopass.pem"
cert_file = pathToGlobus + "usercert.pem"    
    
print("Crawling starting points:")
for i in startDirs:
  print("\t" + i)
print("Starting querying the remote %s server..."%(hostURL))
print("Path to certificates: %s" % (pathToGlobus))

filesToAppend = []
for d in startDirs:
  filesToAppend.extend( spider(hostURL, d, key_file, cert_file) )

uniqueFilesToAppend = []

### UPADTE MODE
if os.path.exists(pathToDb):
  print("DB exists already - entering update mode")
  with open(pathToDb, "r") as currentDB:
    currentDBContentBefore = currentDB.readlines()
    currentDBContent = [line.strip() for line in currentDBContentBefore]
    
    for file in filesToAppend:
      fullPath = hostURL + file
      if fullPath not in currentDBContent:
        print("Adding: %s" % fullPath)
        uniqueFilesToAppend.append(fullPath)
      # else:
        # print("%s is already in DB" % (fullPath))
  with open(pathToDb, "a") as currentDB:
    for u in uniqueFilesToAppend:
      currentDB.write(u + "\n")
### CREATE MODE   
else:
  print("DB is going to be created from scratch")
  with open(pathToDb, "w") as currentDB:
    for f in filesToAppend:
      currentDB.write(hostURL + f + "\n")
      



        
        
        
        
        
        
        
        
        
        
      
