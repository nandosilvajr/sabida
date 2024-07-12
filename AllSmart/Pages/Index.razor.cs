using System.ComponentModel;

namespace AllSmart.Pages;

public partial class Index : ContentPageBase
{
    
    private bool _serverStateOn;
    private bool _pumpOff;

    protected override async void OnParametersSet()
    {
        IsBusy = true;
        
        _serverStateOn = Convert.ToBoolean(await ViewModel.GetDeviceState());

        if (!_serverStateOn)
        {

        }
        
        IsBusy = false;
        
        ViewModel.PropertyChanged += ViewModel_PropertyChanged;
        
        StateHasChanged();
        await base.OnParametersSetAsync();
    }

    private void ViewModel_PropertyChanged(object sender, PropertyChangedEventArgs e)
    {
        StateHasChanged();
    }

    public void ReloadPage()
    {
        OnParametersSet();
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