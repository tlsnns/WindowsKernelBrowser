using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WindowsKernelBrowserUI
{

    class CTLCodeUtility
    {
        public static int GetCTLCode(DeviceType deviceType, int Function, MethodType Method, AccessType Access)
        {
            return ((int)deviceType << 16) | ((int)Access << 14) | (Function << 2) | (int)Method;
        }
    }

    enum DeviceType : int
    {
        FILE_DEVICE_UNKNOWN = 0x00000022
    }
    enum MethodType : int
    {
        METHOD_BUFFERED = 0,
        METHOD_IN_DIRECT = 1,
        METHOD_OUT_DIRECT = 2,
        METHOD_NEITHER = 3,
    }
    enum AccessType : int
    {
        FILE_ANY_ACCESS = 0,
        FILE_READ_ACCESS = 1,// file & pipe
        FILE_WRITE_ACCESS = 2,  // file & pipe
    }
}
