angular.module('directives.ngSuggest', []).directive('ngSuggest', function ($timeout)
{
    return {
        restrict: 'A',
        scope: false,
        link: function (scope, iElement, iAttrs)
        {

            iElement.autocomplete({
                source: scope[iAttrs.ngSuggest],
                select: function (event, ui)
                {
                    $timeout(function ()
                    {
                        iElement.val('');
                        var suggestSelect = scope[iAttrs.suggestSelect];
                        if (suggestSelect instanceof Function) {
                            suggestSelect(ui.item);
                        } else if (undefined != iAttrs.suggestSelect) {
                            throw new Error("Method " + iAttrs.suggestSelect + " not found in scope");
                        }
                    }, 0);
                    return false;
                }
            });
        }
    };
});