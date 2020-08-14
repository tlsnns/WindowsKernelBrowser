using PInvoke;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WindowsKernelBrowserUI
{
    class Device
    {
        Kernel32.SafeObjectHandle SafeObjectHandle;
        Device(string strDeviceName)
        {
            SafeObjectHandle = Kernel32.CreateFile($@"\\.\{strDeviceName}",
                    Kernel32.ACCESS_MASK.GenericRight.GENERIC_READ | Kernel32.ACCESS_MASK.GenericRight.GENERIC_WRITE,
                    Kernel32.FileShare.FILE_SHARE_READ | Kernel32.FileShare.FILE_SHARE_WRITE,
                    Kernel32.SECURITY_ATTRIBUTES.Create(),
                    Kernel32.CreationDisposition.OPEN_EXISTING,
                    Kernel32.CreateFileFlags.FILE_ATTRIBUTE_NORMAL,
                    Kernel32.SafeObjectHandle.Null);

            if (SafeObjectHandle.IsInvalid)
            {
                var errorcode = Kernel32.GetLastError();
                throw new Win32Exception(errorcode, $"打开设备失败,win32错误码：{errorcode}");
            }
        }

        static public Device NewOpenDevice(string strDeviceName)
        {
            return new Device(strDeviceName);
        }

        public void Close()
        {
            SafeObjectHandle.Close();
        }

        public bool DeviceIoControl(int IOCTLCode, byte[] inBuffer, byte[] outBuffer, out int returnLength)
        {
            if (inBuffer == null)
            {
                throw new ArgumentNullException("输入缓存区不可以为null");
            }
            if (outBuffer == null)
            {
                throw new ArgumentNullException("输出缓存区不可以为null");
            }

            bool isSuccess;
            unsafe
            {
                fixed (byte* inBufferPointer = inBuffer)
                {
                    fixed (byte* outBufferPointer = outBuffer)
                    {
                        isSuccess = Kernel32.DeviceIoControl(SafeObjectHandle, IOCTLCode, inBufferPointer, inBuffer.Length,
                            outBufferPointer, outBuffer.Length, out returnLength, null);
                    }
                }
            }
            return isSuccess;
        }
    }
}
