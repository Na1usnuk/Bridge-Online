from django.contrib import admin
from django.shortcuts import get_object_or_404


from .models import Type, ArticleImage, Article
from .forms import ArticleImageForm

class TypeAdmin(admin.ModelAdmin):
    list_display = ('type', 'slug')
    prepopulated_fields = {'slug': ('type',)}
    fieldsets = (
        ('', {
                'fields': ('type', 'slug'), 
        }),    
    )

admin.site.register(Type, TypeAdmin)

class ArticleImageInline(admin.TabularInline):
    model = ArticleImage
    form = ArticleImageForm
    extra = 0
    fieldsets = (
        ('', {
                'fields': ('title', 'image'), 
        }),    
    )

class ArticleAdmin(admin.ModelAdmin):
    list_display = ('title', 'pub_date', 'slug', 'main_page')
    inlines = [ArticleImageInline]
    multiupload_form = True
    multiupload_list = False
    prepopulated_fields = {'slug': ('title',)}
    raw_id_fields = ('type',)
    fieldsets = (
        ('', {
            'fields': ('pub_date', 'title', 'description','main_page'),
            }),
            ((u'Додатково'), {'classes': ('grp-collapse grp-closed',),'fields': ('slug',),
            }),
        )
    def delete_file(self, pk, request):
        obj = get_object_or_404(ArticleImage, pk=pk)
        return obj.delete()

admin.site.register(Article, ArticleAdmin)

            

