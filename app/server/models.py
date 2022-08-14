from distutils.command.upload import upload
from django.db import models
# from .common_imports import *

# Create your models here.
# class Candidate(models.Model):
#         name = models.CharField(max_length=)
#         voting = 
MAX_LENGTH = 2000
MAX_LENGTH_RID = 200
MAX_LENGTH_NAME = 200
class Bulletin1(models.Model):
        c_rid = models.CharField(max_length=MAX_LENGTH)
        c_vote = models.CharField(max_length=MAX_LENGTH)

class Bulletin2(models.Model):
        rid = models.CharField(max_length=MAX_LENGTH_RID,unique=True)
        name = models.CharField(max_length=MAX_LENGTH_NAME)

class UploadImage(models.Model):
        image = models.ImageField(upload_to='images')
        def __str__(self):
                return self.image.name
  
