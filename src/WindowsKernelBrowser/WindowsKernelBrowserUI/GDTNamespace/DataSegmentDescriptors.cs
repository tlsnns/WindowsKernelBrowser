using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using WindowsKernelBrowserUI.GDTNamespace;

namespace WindowsKernelBrowserUI
{
    class DataSegmentDescriptors : NonSystemSegmentDescriptor
    {
        public bool Write { get; private set; }
        public bool ExpandDown { get; set; }
        public bool B { get; private set; }

        DataSegmentDescriptors(byte[] vs, int startIndex) : base(vs, startIndex)
        {
            Write = BitConverter.ToBoolean(vs, startIndex + 14);//14-1
            ExpandDown = BitConverter.ToBoolean(vs, startIndex + 15);//15-1
            B = BitConverter.ToBoolean(vs, startIndex + 22);//22-1
        }
        public static DataSegmentDescriptors Parse(byte[] vs, int startIndex)
        {
            return new DataSegmentDescriptors(vs, startIndex);
        }

        public override Dictionary<string, string> ToDictionaries()
        {
            var d = base.ToDictionaries();
            d.Add("ExpandDown", ExpandDown.ToString());
            d.Add("Write", Write.ToString());
            d.Add("B", B.ToString());
            return d;
        }
    }
}
