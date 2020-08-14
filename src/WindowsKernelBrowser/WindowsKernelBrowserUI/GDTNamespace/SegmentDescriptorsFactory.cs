using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using WindowsKernelBrowserUI.GDTNamespace;

namespace WindowsKernelBrowserUI
{
    class SegmentDescriptorsFactory
    {
        public static GDTEntity Create(byte[] vs, int startIndex)
        {
            var sFlag = (SegmentDescriptorTypes)vs[startIndex + 17];//17-18
            if (sFlag == SegmentDescriptorTypes.NonSystemSegment)
            {
                var cdFlag = (NonSystemSegmentTypes)vs[startIndex + 16];//16-1
                if (cdFlag == NonSystemSegmentTypes.Code)
                {
                    return CodeSegmentDescriptors.Parse(vs, startIndex);
                }
                else
                {
                    return DataSegmentDescriptors.Parse(vs, startIndex);
                }
            }
            else
            {
                return TSSSegmentDescriptors.Parse(vs, startIndex);
            }
        }
    }
}
