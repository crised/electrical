angular.module('services.UserDAO', []).factory("UserDAO", function ($resource, $http)
{
    var CRUDREST = $resource(applicationContextPath + "/rest/user/:id/:action/:relatedEntityId/:subAction", {id: "@id"}, {
        'me': {method: 'GET', params: {id: 'me'}}
    });
    return {
        getAllUsers: function (success, failure)
        {
            CRUDREST.query({}, success, failure);
        },
        getMe: function (success, failure)
        {
            CRUDREST.me({}, success, failure);
        },
        get: function (id, success, failure)
        {
            CRUDREST.get({id: id}, success, failure);
        },
        persistUser: function (user, success, failure)
        {
            var rest = new CRUDREST(user);
            rest.$save(success, failure);
        },
        changePassword: function (currentPassword, newPassword, success, failure)
        {
            var post = $http.post(applicationContextPath + "/rest/user/me/password", "current=" + currentPassword + "&new=" + newPassword,
                    {headers: {'Content-Type': 'application/x-www-form-urlencoded'}});
            if (success instanceof Function) {
                post.success(success)
            }
            if (failure instanceof Function) {
                post.error(failure);
            }
        }
    }
});
