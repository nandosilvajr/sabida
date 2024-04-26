using System.Text.Json.Serialization;

namespace AllSmart.Models
{
    public class SysInfo
    {
        [JsonPropertyName("flashSize")]
        public int FlashSize { get; set; }

        [JsonPropertyName("freeHeap")]
        public int FreeHeap { get; set; }

        [JsonPropertyName("fsTotalBytes")]
        public int FsTotalBytes { get; set; }

        [JsonPropertyName("fsUsedBytes")]
        public int FsUsedBytes { get; set; }
    }
}