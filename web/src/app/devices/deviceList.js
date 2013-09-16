devices.controller('DeviceListCtrl', function ($scope, $location, DeviceDAO)
{
    DeviceDAO.query(function (devices)
    {
        $scope.devices = devices;
    });

    $scope.stats = function (deviceId)
    {
        $location.path('/devices/' + deviceId + '/stats/');
    };
});
