### For converting the html file to arduino syntax because the SPIFFS upload tool isnt working
f=open("esp-frontend.html")
c=f.read()
out=[]
for ch in c:
    if (ch == '\n'):
        out.append('\\n')
    elif ch == '"':
        out.append('\\"')
    else:
        out.append(ch)
f=open('out.conf','w')
f.write(''.join(out))
f.close()