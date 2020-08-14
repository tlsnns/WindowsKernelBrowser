.code
PUBLIC QueryCurrentIDTRAsm

QueryCurrentIDTRAsm   proc
sidt [rcx]
ret
QueryCurrentIDTRAsm ENDP

PUBLIC QueryCurrentGDTRAsm

QueryCurrentGDTRAsm   proc
sgdt [rcx]
ret
QueryCurrentGDTRAsm ENDP

end