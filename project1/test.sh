# make sure python is installed
# chmod +x portal
# PATH=.:$PATH

rm -rf acl.json
portal
portal AddUser
portal AddUser ""
portal AddUser mike
portal AddUser mike ""
portal AddUser mike mikepass
portal AddUser john johnpass
portal AddUser jack jackpass
portal Authenticate
portal Authenticate ""
portal Authenticate mike
portal Authenticate mike ""
portal Authenticate mike badpass
portal Authenticate mike mikepass
portal Authenticate john johnpass
portal SetDomain
portal SetDomain ""
portal SetDomain baduser
portal SetDomain baduser ""
portal SetDomain baduser baddomain
portal SetDomain mike admin
portal SetDomain mike admin
portal SetDomain jack admin
portal SetDomain john user
portal DomainInfo admin
portal DomainInfo user
portal SetType
portal SetType ""
portal SetType badobj
portal SetType badobj ""
portal SetType mulan.mp4 film
portal SetType mulan.mp4 film
portal SetType lor.mp4 film
portal SetType got.mp4 tv
portal TypeInfo film
portal TypeInfo tv
portal AddAccess
portal AddAccess ""
portal AddAccess read
portal AddAccess read ""
portal AddAccess read admin
portal AddAccess read admin ""
portal AddAccess read admin film
portal AddAccess read admin film
portal AddAccess read admin tv
portal AddAccess read user tv
portal CanAccess
portal CanAccess ""
portal CanAccess read
portal CanAccess read ""
portal CanAccess read mike 
portal CanAccess read mike ""
portal CanAccess read john mulan.mp4 
portal CanAccess read john got.mp4 
portal CanAccess read mike mulan.mp4 
portal CanAccess read mike got.mp4 
