import sys
import json

def main():
    if (len(sys.argv) < 2):
        print("Error: missing command")
        exit()

    command = sys.argv[1]
    params = len(sys.argv)

    if(command == "AddUser"):
        if(params < 3):
            print("Error: username missing")
        elif(params < 4):
            print("Error: password missing")
        else:
            add_user(sys.argv[2], sys.argv[3])
    elif(command == "Authenticate"):
        if(params < 3):
            print("Error: missing username")
        elif(params < 4):
            print("Error: missing password")
        else:
            authenticate(sys.argv[2], sys.argv[3])
    elif(command == "SetDomain"):
        if(params < 3):
            print("Error: username missing")
        elif(params < 4):
            print("Error: missing domain")
        else:
            set_domain(sys.argv[2], sys.argv[3])
    elif(command == "DomainInfo"):
        if(params < 3):
            print("Error: missing domain")
        else:
            domain_info(sys.argv[2])
    elif(command == "SetType"):
        if(params < 3):
            print("Error: missing object")
        elif(params < 4):
            print("Error: missing type")
        else:
            set_type(sys.argv[2], sys.argv[3])
    elif(command == "TypeInfo"):
        if(params < 3):
            print("Error: missing type")
        else:
            type_info(sys.argv[2])
    elif(command == "AddAccess"):
        if(params < 3):
            print("Error: missing operation")
        elif(params < 4):
            print("Error: missing domain")
        elif(params < 5):
            print("Error: missing type")
        else:
            add_access(sys.argv[2], sys.argv[3], sys.argv[4])
    elif(command == "CanAccess"):
        if(params < 3):
            print("Error: missing operation")
        elif(params < 4):
            print("Error: missing user")
        elif(params < 5):
            print("Error: missing object")
        else:
            can_access(sys.argv[2], sys.argv[3], sys.argv[4])
    else:
        print("Error: Command not recognized")

def add_user(user, password):
    if(user == ""):
        print("Error: username missing")
    elif(user in access["users"]):
        print("Error: user exists")
    else:
        access["users"][user] = password
        print("Success")

def authenticate(user, password):
    if(user not in access["users"]):
        print("Error: no such user")
    elif(access["users"][user] != password):
        print("Error: bad password")
    else:
        print("Success")

def set_domain(user, domain):
    if(domain == ""):
        print("Error: missing domain")
    elif(user not in access["users"]):
        print("Error: no such user")
    elif(domain in access["domains"]):
        if user not in access["domains"][domain]:
            access["domains"][domain].append(user)
        print("Success")
    else:
        access["domains"][domain] = [user]
        print("Success")
        
def domain_info(domain):
    if(domain == ""):
        print("Error: missing domain")
    else:
        if(domain in access["domains"]):
            for user in access["domains"][domain]:
                print(user)

def set_type(object_name, type_name):
    if(object_name == ""):
        print("Error: objectname cannot be null")
    elif(type_name == ""):
        print("Error: type cannot be null")
    elif(type_name in access["types"]):
        if object_name not in access["types"][type_name]:
            access["types"][type_name].append(object_name)
        print("Success")
    else:
        access["types"][type_name] = [object_name]
        print("Success")

def type_info(type_name):
    if(type_name == ""):
        print("Error: missing type")
    else:
        if(type_name in access["types"]):
            for obj in access["types"][type_name]:
                print(obj)

def add_access(operation, domain_name, type_name):
    if(operation == ""):
        print("Error: missing operation")
    elif(domain_name == ""):
        print("Error: missing domain")
    elif(type_name == ""):
        print("Error: missing type")
    else:
        if(domain_name not in access["domains"]):
            access["domains"][domain_name] = []
        if(type_name not in access["types"]):
            access["types"][type_name] = []
        if domain_name not in access["access_permissions"]:
            access["access_permissions"][domain_name] = dict()
        if type_name not in access["access_permissions"][domain_name]:
            access["access_permissions"][domain_name][type_name] = []
        if operation not in access["access_permissions"][domain_name][type_name]:
            access["access_permissions"][domain_name][type_name].append(operation)
        print("Success")

def can_access(operation, user, object_name):
    if(operation == ""):
        print("Error: missing operation")
    elif(user == ""):
        print("Error: missing user")
    elif(object_name == ""):
        print("Error: missing object")
    else:
        user_domains = []
        for domain in access["domains"]:
            if(user in access["domains"][domain]):
                user_domains.append(domain)
        object_types = []
        for t in access["types"]:
            if(object_name in access["types"][t]):
                object_types.append(t)

        for domain in user_domains:
            for t in object_types:
                if(t in access["access_permissions"][domain]):
                    if(operation in access["access_permissions"][domain][t]):
                        print("Success")
                        return
        
        print("Error: access denied")

def write_out():
    with open('access.json', 'w') as f:
        json.dump(access, f)

try:
    with open('access.json') as f:
        access = json.load(f)
except:
    access = dict()

# Check init attr of access
if "users" not in access:
    access["users"] = dict()
if "domains" not in access:
    access["domains"] = dict()
if "types" not in access:
    access["types"] = dict()
if "access_permissions" not in access:
    access["access_permissions"] = dict()

main()
write_out()