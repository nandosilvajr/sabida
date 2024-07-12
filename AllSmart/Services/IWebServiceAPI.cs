#nullable enable
using Refit;
using AllSmart.Models;

namespace AllSmart.Services;

public interface IWebServiceApi
{
    [Get("/$sysInfo")]
    Task<SysInfo?> GetSysInfo();
    
    [Get("/pumpStatusInfo")]
    Task<int?> GetPumpStatus();
    
    [Get("/$readSensor")]
    Task<int?> GetSensorData();

    [Get("/$getSensorState")]
    Task<string> GetSensorState();

    [Post("/$pumpCommand")]
    Task<int?> ChangePumpState();

    [Post("/$selectDays")]
    Task SelectWeekDays([Body] string weekDays );

    [Get("/$getWeekDays")]
    Task<string> GetWeekDays();

    [Get("/$wateringTimeUpdate")]
    Task<int?> WateringTimeUpdate();

    [Post("/$wateringTime")]
    Task<int?> WateringTime([Body]string value);

    [Get("/$getWateringTime")]
    Task<string> GetWateringTime();

    [Get("/$getLastWatering")]
    Task<string> GetLastWatering();
}