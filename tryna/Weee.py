import random


cipher2 = "abcdefghijklmnopqrestuvwxyz"
cipher = "abcdefghijklmnopqrestuvwxyz1234567t890!s@#ozu7mv$a%^ee&gf*4h(ewssc5p)~!s$_+qk`-is=[j]e*cg-fw;4q\j-dpyuv`s5,x.p@0/oh2$mkuop%6+vws)r1$s{m~q3!s}s#cp-t08:|[5;$f@hsg%swe&3-t8;@sc]`_bky1te}n!5wp30fc%k4`n[!2s=t-$|~^8vsrk`-t0u.*cfo-rg6`p[}20h[_ss8t/l49[~\bdh@#`;sk02($t5uweg26!,opc0nsml|\3s-fztk7mss7z\$z0iw^an]u7ek-jqz3&_t)1wt4;\0d!yarej^$#w*:k5.s"
check = ""
ans = ""
cnt = 0

while True:
    test2 = open("ciphers.txt", "a")
    test = open("ezwords.txt", "r")
    line = test.readline()
    for c in cipher:
        if c not in check:
            check += c
            ans += chr(random.randint(97, 122))
        elif c in check:
            ans += ans[check.find(c)]
    while line:
        temp = line.rstrip('\n')
        if temp in ans:
            ans = ans[:ans.find(temp)] + ' ' + ans[ans.find(temp):]
            ans = ans[:ans.find(temp)+len(temp)] + ' ' + ans[ans.find(temp)+len(temp):]
            cnt += 1
        if cnt > 29:
            print("found")
            test2.write(ans + "\n")
            break
        line = test.readline()
    cnt = 0
    check = ""
    ans = ""
    test2.close()




