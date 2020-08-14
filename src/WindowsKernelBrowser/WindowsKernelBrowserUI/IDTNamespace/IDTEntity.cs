using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WindowsKernelBrowserUI
{
    class IDTEntity
    {
        public int VectorNumber { get; set; }
        public ushort InterruptServiceRoutineSegmentSelector { get; set; }
        public ulong InterruptServiceRoutineOffset { get; set; }
        public byte InterruptStackTableIndex { get; set; }
        public SystemSegmentAndGateDescriptorType TYPE { get; set; }
        public byte DescriptorPrivilegeLevel { get; set; }
        public bool SegmentPresentFlag { get; set; }


        IDTEntity(int vectorNumber, byte[] vs, int startIndex)
        {
            VectorNumber = vectorNumber;
            InterruptServiceRoutineSegmentSelector = BitConverter.ToUInt16(vs, startIndex);//0-2
            InterruptServiceRoutineOffset = BitConverter.ToUInt64(vs, startIndex + 2);//2-8
            InterruptStackTableIndex = vs[startIndex + 10];//10-1
            TYPE = (SystemSegmentAndGateDescriptorType)vs[startIndex + 11];//11-1
            DescriptorPrivilegeLevel = vs[startIndex + 12];//12-1
            SegmentPresentFlag = BitConverter.ToBoolean(vs, startIndex + 13);//13-1
        }

        public static IDTEntity Parse(int VectorNumber, byte[] vs, int startIndex)
        {
            return new IDTEntity(VectorNumber, vs, startIndex);
        }
    }
}
