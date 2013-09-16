angular.module('services.PackageDAO', []).factory("PackageDAO", function ($resource)
{
    var PackageREST = $resource(applicationContextPath + "/api/package/:id/:action/:relatedEntityId/:subAction", {id: "@id"}, {
        'diff': {method: 'GET', params: {action: 'rev', subAction: 'diff'}},
        'history': {method: 'GET', isArray: true, params: {action: 'history'}},
        'query': {method: 'GET', isArray: true},
        'contents': {method: 'GET', isArray: true, params: {action: "contents"}},
        'move': {method: 'POST', params: {action: 'move', relatedEntityId: '@parentId'}},
        'moveToChangelist': {method: 'POST', params: {action: 'changelist', relatedEntityId: '@changelistId'}}
    });
    return {
        contents: function (parentId, success, failure)
        {
            PackageREST.contents({id: parentId}, success, failure);
        },
        persistPackage: function (pkg, callback)
        {
            var restPkg = new PackageREST(pkg);
            restPkg.$save(function (result)
            {
                angular.extend(pkg, result);
                if (callback instanceof Function) {
                    callback(pkg);
                }
            });
        },
        getDiff: function (packageId, changeRequestId, success, error)
        {
            PackageREST.diff({id: packageId, relatedEntityId: changeRequestId}, success, error);
        },
        getPackage: function (packageId, success, error)
        {
            PackageREST.get({id: packageId}, success, error);
        },
        getHistory: function (pkgId, success, error)
        {
            PackageREST.history({id: pkgId}, success, error);
        },
        move: function (packageId, parentId, success)
        {
            PackageREST.move({id: packageId, parentId: parentId}, success);
        },
        moveToChangelist: function (packageId, changelistId, success, failure)
        {
            PackageREST.moveToChangelist({id: packageId, changelistId: changelistId}, success, failure);
        },
        removePackage: function (packageId, callback)
        {
            var restPkg = new PackageREST({id: packageId});
            restPkg.$delete(function ()
            {
                if (callback instanceof Function) {
                    callback(packageId);
                }
            });
        }
    }
});
