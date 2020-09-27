import sys

def main():
    command = sys.argv[1]
    if command == "AddUser":
        add_user(sys.argv[2], sys.argv[3])
    elif command == "Authenticate":
        authenticate(sys.argv[2], sys.argv[3])
    elif command == "SetDomain":
        set_domain(sys.argv[2], sys.argv[3])
    elif command == "DomainInfo":
        domain_info(sys.argv[2])
    elif command == "SetType":
        set_type(sys.argv[2], sys.argv[3])
    elif command == "TypeInfo":
        type_info(sys.argv[2])
    elif command == "AddAccess":
        add_access(sys.argv[2], sys.argv[3], sys.argv[4])
    elif command == "CanAccess":
        can_access(sys.argv[2], sys.argv[3], sys.argv[4])
    else:
        print("Error: Command not recognized")

def add_user(user, password):
    print("Success")

def authenticate(user, password):
    print("Success")

def set_domain(user, domain):
    print("Success")

def domain_info(domain):
    print("Success")

def set_type(objectname, type):
    print("Success")

def type_info(type):
    print("Success")

def add_access(operation, domain_name, type_name):
    print("Success")

def can_access(operation, user, object):
    print("Success")