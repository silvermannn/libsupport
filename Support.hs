{-# LANGUAGE CApiFFI #-}

module Support.Support where

import Foreign.C
import Foreign.Ptr
import Foreign.Storable
import Foreign.Marshal.Utils
import Foreign.Marshal.Array

foreign import capi "Support.h parse" parse' :: CString -> CString -> IO CBool
foreign import capi "Support.h saveSentences" saveSentences' :: CString -> IO CBool
foreign import capi "Support.h loadSentences" loadSentences' :: CString -> IO CBool
foreign import capi "Support.h saveEncoder" saveEncoder' :: CString -> IO CBool
foreign import capi "Support.h loadEncoder" loadEncoder' :: CString -> IO CBool

foreign import capi "Support.h trainTagger" trainTagger' :: CFloat -> IO CBool
foreign import capi "Support.h saveTagger" saveTagger' :: CString -> IO CBool
foreign import capi "Support.h loadTagger" loadTagger' :: CString -> IO CBool

foreign import capi "Support.h trainTreeBuilder" trainTreeBuilder' :: CFloat -> IO CBool
foreign import capi "Support.h saveTreeBuilder" saveTreeBuilder' :: CString -> IO CBool
foreign import capi "Support.h loadTreeBuilder" loadTreeBuilder' :: CString -> IO CBool

foreign import capi "Support.h index2word" index2word' :: CULong -> Ptr CString -> IO CBool
foreign import capi "Support.h word2index" word2index' :: CString -> Ptr CULong -> IO CBool

foreign import capi "Support.h tag" tag' :: Ptr CULong -> CULong -> Ptr CULong -> IO CBool

foreign import capi "Support.h getCompoundPOSTag" getCompoundPOSTag' :: CULong -> Ptr CULong -> Ptr CULong -> IO CBool
foreign import capi "Support.h index2POSTag" index2POSTag' :: CULong -> Ptr CString -> IO CBool
foreign import capi "Support.h index2FeatureName" index2FeatureName' :: CULong -> Ptr CString -> IO CBool
foreign import capi "Support.h index2FeatureValue" index2FeatureValue' :: CULong -> Ptr CString -> IO CBool

foreign import capi "Support.h buildDependencyTree" buildDependencyTree' :: Ptr CULong -> CULong -> Ptr CULong -> IO CBool
foreign import capi "Support.h getCompoundDeprelTag" getCompoundDeprelTag' :: CULong -> Ptr CULong -> Ptr CULong -> IO CBool
foreign import capi "Support.h index2dependencyRelation" index2dependencyRelation' :: CULong -> Ptr CString -> IO CBool
foreign import capi "Support.h index2dependencyRelationModifier" index2dependencyRelationModifier' :: CULong -> Ptr CString -> IO CBool

parsePath :: FilePath -> String -> IO Bool
parsePath path parser = do
    cpath <- newCString path
    cparser <- newCString parser
    res <- parse' cpath cparser
    return $ toBool res

saveSentences :: FilePath -> IO Bool
saveSentences path = do
    cpath <- newCString path
    res <- saveSentences' cpath
    return $ toBool res

loadSentences :: FilePath -> IO Bool
loadSentences path = do
    cpath <- newCString path
    res <- loadSentences' cpath
    return $ toBool res

saveEncoder :: FilePath -> IO Bool
saveEncoder path = do
    cpath <- newCString path
    res <- saveEncoder' cpath
    return $ toBool res

loadEncoder :: FilePath -> IO Bool
loadEncoder path = do
    cpath <- newCString path
    res <- loadEncoder' cpath
    return $ toBool res

trainTagger :: Float -> IO Bool
trainTagger sf = do
    res <- trainTagger' (realToFrac sf)
    return $ toBool res

trainTreeBuilder :: Float -> IO Bool
trainTreeBuilder sf = do
    res <- trainTreeBuilder' (realToFrac sf)
    return $ toBool res

saveTagger :: FilePath -> IO Bool
saveTagger path = do
    cpath <- newCString path
    res <- saveTagger' cpath
    return $ toBool res

loadTagger :: FilePath -> IO Bool
loadTagger path = do
    cpath <- newCString path
    res <- loadTagger' cpath
    return $ toBool res

saveTreeBuilder :: FilePath -> IO Bool
saveTreeBuilder path = do
    cpath <- newCString path
    res <- saveTreeBuilder' cpath
    return $ toBool res

loadTreeBuilder :: FilePath -> IO Bool
loadTreeBuilder path = do
    cpath <- newCString path
    res <- loadTreeBuilder' cpath
    return $ toBool res

index2word :: Int -> IO (Maybe String)
index2word = index2string index2word'

word2index :: String -> IO (Maybe Int)
word2index = string2index word2index'

tag :: [Int] -> IO (Maybe [Int])
tag ws = do
    css <- callocArray size
    pokeArray css $ map toEnum ws
    ts <- callocArray size
    res <- tag' css (toEnum size) ts
    if toBool res then do
        tags <- peekArray size ts
        return $ Just $ map fromEnum tags
    else return Nothing
    where
        size = length ws

getCompoundPOSTag :: Int -> IO (Maybe [Int])
getCompoundPOSTag = getCompoundTag getCompoundPOSTag' 32

index2POSTag :: Int -> IO (Maybe String)
index2POSTag = index2string index2POSTag'

index2FeatureName :: Int -> IO (Maybe String)
index2FeatureName = index2string index2FeatureName'

index2FeatureValue :: Int -> IO (Maybe String)
index2FeatureValue = index2string index2FeatureValue'

buildDependencyTree :: [Int] -> IO (Maybe [Int])
buildDependencyTree ss = do
    ts <- callocArray size
    pokeArray ts $ map toEnum ss
    es <- callocArray (3 * size)
    res <- buildDependencyTree' ts (toEnum size) es
    if toBool res then do
        edges <- peekArray (3 * size) es
        return $ Just $ map fromEnum edges
    else return Nothing
    where
        size = length ss

getCompoundDeprelTag :: Int -> IO (Maybe [Int])
getCompoundDeprelTag = getCompoundTag getCompoundDeprelTag' 32

index2dependencyRelation :: Int -> IO (Maybe String)
index2dependencyRelation = index2string index2dependencyRelation'

index2dependencyRelationModifier :: Int -> IO (Maybe String)
index2dependencyRelationModifier = index2string index2dependencyRelationModifier'

-- -------------------------------------------------------------

getCompoundTag :: (CULong -> Ptr CULong -> Ptr CULong -> IO CBool) -> Int -> Int -> IO (Maybe [Int])
getCompoundTag f size t = do
    plen <- new (toEnum size)
    parr <- mallocArray size
    res <- f (toEnum t) parr plen
    if toBool res then do
        size' <- peek plen
        ts <- peekArray (fromEnum size') parr
        return $ Just $ map fromEnum ts
    else return Nothing

index2string ::  (CULong -> Ptr CString -> IO CBool) -> Int -> IO (Maybe String)
index2string f ix = do
    ps <- newCString ""
    p <- new ps
    res <- f (toEnum ix) p
    if toBool res then do
        ps' <- peek p
        ss <- peekCString ps'
        return $ Just ss
    else return Nothing

string2index ::  (CString -> Ptr CULong -> IO CBool) -> String -> IO (Maybe Int)
string2index f s = do
    ps <- newCString s
    p <- new 0
    res <- f ps p
    if toBool res then do
        ix <- peek p
        return $ Just $ fromEnum ix
    else return Nothing
