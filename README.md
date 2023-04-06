# MT_SC-with-Win32-API

WiP

This code/tool attempts to mimic the Microsoft Windows utility sc.exe using C++ and Win32 API. The code/tool in this repository is more or less a work in progress and may or may not be continually improved. Tool/Code so far only contains the create, delete, start, stop, failure, qdescription, query, and config functions. Since this code mimics sc.exe, it can be used in the same way (ex: sc.exe query = "MT_SC-with-Win32-API.exe" query).


## How to Use:

*Will need to be run as Admin*

### Create

DESCRIPTION: Creates a service

"MT_SC-with-Win32-API.exe" create [Service Name] binPath= [Binary Path] <option1> <option2>...

OPTIONS:  
    
    - type= {own | share | kernel | filesys | rec | adapt | interact type= {own | share}}
    - start= {boot | system | auto | demand | disabled | delayed-auto}
    - error= {normal | severe | critical | ignore}
    - binpath= <BinaryPathName>
    - group= <LoadOrderGroup>
    - tag= {yes | no}
    - depend= <dependencies>
    - obj= {<AccountName> | <ObjectName>}
    - displayname= <DisplayName>
    - password= <Password>


  
  

### Delete

DESCRIPTION: Deletes a service

"MT_SC-with-Win32-API.exe" delete [Service Name]

  
  

### Start

DESCRIPTION: Starts a service

"MT_SC-with-Win32-API.exe" start [Service Name]


  
  
### Stop

DESCRIPTION: Stops a service

"MT_SC-with-Win32-API.exe" stop [Service Name]


  
  
### Failure

DESCRIPTION: Configures the failure actions for a service

"MT_SC-with-Win32-API.exe" failure [<ServiceName>] <option1> <option2>

OPTIONS:  
    
    - reset= <ErrorFreePeriod>
    - reboot= <BroadcastMessage>
    - command= <CommandLine>
    - actions= {"" | {[run/<MS>] | [restart/<MS>] | [reboot/<MS>]}[/...]

  
  

### qdescription

DESCRIPTION: Gets the description for a service

"MT_SC-with-Win32-API.exe" qdescription [Service Name]

  
  

### Query

DESCRIPTION: Starts a service

"MT_SC-with-Win32-API.exe" query [Service Name] <option1> <option2>

OPTIONS:  
    
    - type= {driver | service | all}
    - type= {own | share | interact | kernel | filesys | rec | adapt}
    - state= {active | inactive | all}
    - bufsize= <BufferSize>
    - ri= <ResumeIndex>
    - group= <GroupName>
  

### Config

DESCRIPTION: Makes configuration changes to a service

"MT_SC-with-Win32-API.exe" [Service Name] <option1> <option2>...

OPTIONS:  
    
    - type= {own | share | kernel | filesys | rec | adapt | interact type= {own | share}}
    - start= {boot | system | auto | demand | disabled | delayed-auto}
    - error= {normal | severe | critical | ignore}
    - binpath= <BinaryPathName>
    - group= <LoadOrderGroup>
    - tag= {yes | no}
    - depend= <dependencies>
    - obj= {<AccountName> | <ObjectName>}
    - displayname= <DisplayName>
    - password= <Password>


