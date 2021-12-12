from django.db import models
# from .common_imports import *

# Create your models here.
# class Candidate(models.Model):
#         name = models.CharField(max_length=)
#         voting = 
MAX_LENGTH = 2000
MAX_LENGTH_RID = 200
MAX_LENGTH_NAME = 200
class BB1(models.Model):
        c_rid = models.CharField(max_length=MAX_LENGTH)
        c_u = models.CharField(max_length=MAX_LENGTH)

class BB2(models.Model):
        rid = models.CharField(max_length=MAX_LENGTH_RID)
        name = models.CharField(max_length=MAX_LENGTH_NAME)


