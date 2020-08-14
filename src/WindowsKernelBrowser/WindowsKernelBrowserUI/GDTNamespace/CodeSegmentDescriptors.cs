using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WindowsKernelBrowserUI
{
    class CodeSegmentDescriptors : NonSystemSegmentDescriptor
    {
        public bool Read { get; private set; }
        public bool Conforming { get; private set; }
        public bool CodeSegment64Bit { get; private set; }
        public bool D { get; private set; }

        CodeSegmentDescriptors(byte[] vs, int startIndex) : base(vs, startIndex)
        {
            Read = BitConverter.ToBoolean(vs, startIndex + 14);//14-1
            Conforming = BitConverter.ToBoolean(vs, startIndex + 15);//15-1

            CodeSegment64Bit = BitConverter.ToBoolean(vs, startIndex + 21);//21-1

            D = BitConverter.ToBoolean(vs, startIndex + 22);//22-1
        }
        public static CodeSegmentDescriptors Parse(byte[] vs, int startIndex)
        {
            return new CodeSegmentDescriptors(vs, startIndex);
        }

        public override Dictionary<string, string> ToDictionaries()
        {
            var d = base.ToDictionaries();
            d.Add("Read", Read.ToString());
            d.Add("Conforming", Conforming.ToString());
            d.Add("CodeSegment64Bit", CodeSegment64Bit.ToString());
            d.Add("D", D.ToString());
            return d;
        }
    }
}
