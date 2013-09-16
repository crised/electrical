angular.module('directives.ngFocus', []).directive('ngFocus', function ()
{
    return {
        link: function (scope, element, attrs, controller)
        {
            element[0].focus();
        }
    };
});