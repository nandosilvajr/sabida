using System.Diagnostics;
using AllSmart.Services;
using Microsoft.AspNetCore.Components;
using AllSmart.Models;

namespace AllSmart.Pages;

public partial class Index : ContentPageBase
{
    private bool _serverState;
    private bool _pumpOff;

    protected override async void OnParametersSet()
    {
        IsBusy = true;
        
        _serverState = Convert.ToBoolean(await GetDeviceState());
        
        IsBusy = false;

        StateHasChanged();
        await base.OnParametersSetAsync();
    }
    
    private async void OnPumpChange()
    {
        var response = await WebServiceApi.ChangePumpState();
        if(response.HasValue)
        {
            _pumpOff = Convert.ToBoolean(response.Value);
            StateHasChanged();
        }
    }

}