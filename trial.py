from datetime import datetime

t= datetime.now()
a = int(datetime.now().strftime('%H%M%S'))
print(a)

for i in range (0,10):
    print(i)

t1= datetime.now()
# a1 = int(t1.strftime('%H%M%S'))
a1 = a+9
print(a1)

if a1-a<10:
    print(a1-a)
    for i in range (0,10):
        print(i)