using AllSmart.Services;
using Microsoft.AspNetCore.Components;
using AllSmart.Models;

namespace AllSmart.Pages;

public partial class Index : ContentPageBase
{
   

    private int SensorData { get; set; }

    private bool SensorState { get; set; }

    private bool PumpOff { get; set; } = true;

    private bool AlarmOn { get; set; }
    protected override async void OnParametersSet()
    {
        IsBusy = true;
        try
        {
           
            var sensorState = await GetSensorState();

            if (sensorState.HasValue)
            {
                SensorState = sensorState.Value;
            }


            if (!SensorState)
            {
                SensorData = await GetSensorData();
            }else
            {
                SensorData = 1;
            }
        }
        catch (Exception e)
        {
            Console.WriteLine(e);
            IsBusy = false;
            throw;
        }

        IsBusy = false;

        StateHasChanged();
        await base.OnParametersSetAsync();
    }


    private async void OnPumpChange()
    {
        var response = await WebServiceApi.ChangePumpState();
        if(response.HasValue)
        {
            PumpOff = Convert.ToBoolean(response.Value);
            StateHasChanged();
        }
    }

}