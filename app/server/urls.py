from django.urls import path

from . import views

urlpatterns = [
	path('candidate',views.candidate,name='candidate'),
	path('vvpr_receipt',views.vvpr_receipt,name='vvpr_receipt'),
	path('voter_receipt',views.voter_receipt,name='voter_receipt'),
	path('evm_vvpr_receipt',views.evm_vvpr_receipt,name='evm_vvpr_receipt'),
	path('index',views.index,name='index'),
	path('',views.voter,name="voter"),
	# path('', views.index, name='index'),
]