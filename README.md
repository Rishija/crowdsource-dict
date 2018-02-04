 # Server Dictionary
 #### A crowdsource dictionary for developers. Words in dictionary being frequent programming errors.
 ### Details
 While server is in listening state, client requests to create connection. On successful connection client is asked for his username. If username is unique, client have access to the dictionary else connection is terminated. Once connected, client can perform following operations:
 - **Add error to the dictionary:** Only admin is allowed to do so. If permission is granted, details of the error are saved in the database (in the form of separate files for each error).
 - **Find error:** Any client can get information about errors present in the dictionary
 - **Prefix search:** Get list of errors with given prefix. Errors with maximum number of search top the list.
 ### Implementation
 - Server is written in python using sockets to create connection between server and the client.
 - Dictionary is implemented in c++ using `Tries` data structure.
 ### Download and Run
 ``` bash
 $ git clone https://github.com/Rishija/serverDict
 $ cd serverDict
 
 # Server side
 $ python server.py
 
 # Client side
 $ python client.py localhost
 ```
 ### Sceenshots
 ![admin](https://i.imgur.com/ZAjQXGe.jpg)
 ![client](https://i.imgur.com/DPFaHAK.jpg)
 ![server](https://i.imgur.com/HUAzwbO.jpg)
