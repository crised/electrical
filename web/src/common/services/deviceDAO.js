angular.module('services.DeviceDAO', []).factory("DeviceDAO", function ($resource)
{
    var REST = $resource(applicationContextPath + "/rest/device/:id/:controller/:start/:end", {id: "@id"}, {
        'stats': {method: 'GET', params: {controller: 'stats'}}
    });
    return {
        getDevices: function (success, failure)
        {
            REST.query({}, success, failure);
        },
        getStats: function (deviceId, startDate, endDate, success, failure)
        {
            REST.stats({id: deviceId, start: startDate.getTime(), end: endDate.getTime()}, success, failure);
        }
    };
});
