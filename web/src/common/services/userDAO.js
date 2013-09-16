angular.module('services.UserDAO', []).factory("UserDAO", function ($resource, $http)
{
    var CRUDREST = $resource(applicationContextPath + "/api/user/:id/:action/:relatedEntityId/:subAction", {id: "@id"}, {
        'me': {method: 'GET', params: {id: 'me'}}
    });
    return {
        getMe: function (success, failure)
        {
            CRUDREST.me({}, success, failure);
        },
        persistUser: function (usecase, success, failure)
        {
            var restUsecase = new CRUDREST(usecase);
            restUsecase.$save(success, failure);
        },
        changePassword: function (currentPassword, newPassword, success, failure)
        {
            var post = $http.post(applicationContextPath + "/api/user/me/password", "current=" + currentPassword + "&new=" + newPassword,
                    {headers: {'Content-Type': 'application/x-www-form-urlencoded'}});
            if (success instanceof Function) {
                post.success(success)
            }
            if (failure instanceof Function) {
                post.error(failure);
            }
        },
        register: function (email, password, success, failure)
        {
            var post = $http.post(applicationContextPath + "/api/user", "email=" + email + "&password=" + password,
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
