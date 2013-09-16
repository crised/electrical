angular.module('services.DeviceDAO', []).factory("DeviceDAO", function ($resource)
{
    return $resource(applicationContextPath + "/rest/device/:id", {id: "@id"}, {});
});
