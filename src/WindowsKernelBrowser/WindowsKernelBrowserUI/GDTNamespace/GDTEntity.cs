using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using WindowsKernelBrowserUI.GDTNamespace;

namespace WindowsKernelBrowserUI
{
    abstract class GDTEntity
    {
        public byte Index { get; private set; }
        public uint Limit { get; private set; }
        public ulong Address { get; private set; }
        public SegmentDescriptorTypes SegmentDescriptorType { get; private set; }
        public DescriptorPrivilegeLevelType DescriptorPrivilegeLevel { get; private set; }
        public bool Present { get; private set; }
        public byte AvailableAndReservedBit { get; private set; }
        public GranularityTypes Granularity { get; private set; }

        public GDTEntity(byte[] vs, int startIndex)
        {
            Index = vs[startIndex];//0-1
            Limit = BitConverter.ToUInt32(vs, startIndex + 1);//1-4
            Address = BitConverter.ToUInt64(vs, startIndex + 5);//5-8

            SegmentDescriptorType = (SegmentDescriptorTypes)vs[startIndex + 17];//17-18
            DescriptorPrivilegeLevel = (DescriptorPrivilegeLevelType)vs[startIndex + 18];//18-1
            Present = BitConverter.ToBoolean(vs, startIndex + 19);//19-1
            AvailableAndReservedBit = vs[startIndex + 20];//20-1

            Granularity = (GranularityTypes)vs[startIndex + 23];//23-1
        }
        public virtual Dictionary<string, string> ToDictionaries()
        {
            Dictionary<string, string> keyValuePairs = new Dictionary<string, string>();
            keyValuePairs.Add("Index", Index.ToString("X"));
            keyValuePairs.Add("Limit", Limit.ToString("X"));
            keyValuePairs.Add("Address", Address.ToString("X"));
            keyValuePairs.Add("SegmentDescriptorType", SegmentDescriptorType.ToString());
            keyValuePairs.Add("DescriptorPrivilegeLevel", DescriptorPrivilegeLevel.ToString());
            keyValuePairs.Add("Present", Present.ToString());
            keyValuePairs.Add("AvailableAndReservedBit", AvailableAndReservedBit.ToString());
            keyValuePairs.Add("Granularity", Granularity.ToString());
            return keyValuePairs;
        }
    }
}
