var ð= "\x71\x11\x24\x59\x8d\x6d\x71\x11\x35\x16\x8c\x6d\x71\x0d\x39\x47\x1f\x36\xf1\x2f\x39\x36\x8e\x3c\x4b\x39\x35\x12\x87\x7c\xa3\x10\x74\x58\x16\xc7\x71\x56\x68\x51\x2c\x8c\x73\x45\x32\x5b\x8c\x2a\xf1\x2f\x3f\x57\x6e\x04\x3d\x16\x75\x67\x16\x4f\x6d\x1c\x6e\x40\x01\x36\x93\x59\x33\x56\x04\x3e\x7b\x3a\x70\x50\x16\x04\x3d\x18\x73\x37\xac\x24\xe1\x56\x62\x5b\x8c\x2a\xf1\x45\x7f\x86\x07\x3e\x63\x47";

function XOR(x,y)
{
    return x^y;
}

function SUMFROM0(y)
{
    var z = 0;
    for(var i=0;i<y;i++)
    {
        z += Math.pow(2,i);
    }
    return z;
}

function SUMFROM8(y)
{
    var z = 0;
    for(var i=8-y;i<8;i++)
    {
        z += Math.pow(2,i);
    }
    return z
}

/*
function tp(x,y)
{
    y = y % 8;
    Ï = SUMFROM0(y);
    Ï = (x & Ï) << (8-y);
    return (Ï) + (x >> y);
}
*/

function rot(x,y)
{
    y = y % 8;
    Ï = SUMFROM8(y);
    Ï = (x & Ï) >> (8-y);
    return ((Ï) + (x << y)) & 0x00ff;
}

function unknow2(x,y)
{
    return rot(x,y)
}

function unknow(var1,key)
{
    tmp1 = "";
    tmp12 = "";
    for(var i=0;i<var1.length;i++)
    {
        c = var1.charCodeAt(i);
        if(i != 0)
        {
            t = tmp1.charCodeAt(i-1)%2;
            switch(t)
            {
                case 0:cr = XOR(c, key.charCodeAt(i % key.length));
                break;
                case 1:cr = unknow2(c, key.charCodeAt(i % key.length));
                break;
            }
        }
        else
        {
            cr = XOR(c, key.charCodeAt(i % key.length));
        }
        tmp1 += String.fromCharCode(cr);
    }
    return tmp1;
}

function main(þ){
    var ŋ=0;
    for(var i=0;i<þ.length;i++)
    {
        ŋ+=þ["charCodeAt"](i)
    }
    if(ŋ==8932)
    {
        var ç=window.open("","","\x77\x69\x64\x74\x68\x3d\x33\x30\x30\x2c\x68\x65\x69\x67\x68\x74\x3d\x32\x20\x30");
        ç.document.write(þ)
    }
    else{alert("Mauvais mot de passe!")
}
}
main(unknow(ð,prompt("Mot de passe?")));