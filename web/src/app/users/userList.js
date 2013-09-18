users.controller('UserListCtrl', function ($scope, $location, Authenticator, MessageFactory, UserDAO)
{
    $scope.users = [];

    UserDAO.getAllUsers(function (users)
    {
        $scope.users = users;
    });

    $scope.edit = function (user)
    {
        $location.path('/users/' + user.id);
    };

    $scope.register = function (user)
    {
        $location.path('/users/register');
    };

    $scope.createUser = function ()
    {
        UserDAO.persistUser(this.user, function ()
        {
            MessageFactory.info("User saved successfully");
        });
    };

});
