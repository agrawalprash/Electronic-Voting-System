from dataclasses import field
from django import forms
from .models import UploadImage


class Voterform(forms.Form):
        voter_id = forms.IntegerField()

# class VotingForm(forms.Form):
#         def __init__(self, voter_id, candidate_list, candidate_wm):
#                 super().__init__()
#                 self.voter_id = voter_id
#                 self.candidate_list = candidate_list
#                 self.wm = candidate_wm
#         voting = forms.ChoiceField(choices=[(self.candidate_list[i],str(self.wm[i])) for i in range(len(self.candidate_list))])


class UserImageForm(forms.ModelForm):
        class Meta:
                model = UploadImage
                fields = '__all__'