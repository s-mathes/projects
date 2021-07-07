package com.hp.sms.wsuiotdevice;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import software.amazon.awssdk.auth.credentials.AwsBasicCredentials;
import software.amazon.awssdk.auth.credentials.AwsCredentials;
import software.amazon.awssdk.auth.credentials.AwsSessionCredentials;
import software.amazon.awssdk.auth.credentials.AwsSessionCredentials.*;
import software.amazon.awssdk.auth.credentials.StaticCredentialsProvider;
import software.amazon.awssdk.core.SdkRequest;
import software.amazon.awssdk.regions.Region;
import software.amazon.awssdk.services.cloudhsm.model.ListAvailableZonesRequest;
import software.amazon.awssdk.services.iot.*;
import com.amazonaws.services.iot.client.AWSIotMqttClient;
import com.hp.sms.wsuiotdevice.sampleUtil.SampleUtil;
import software.amazon.awssdk.services.iot.model.SearchIndexRequest;
import software.amazon.awssdk.services.iot.model.SearchIndexResponse;

import java.util.ArrayList;
import java.util.List;
import java.util.function.Consumer;

public class FleetQuery {
    /*private final String clientId;
    private final String clientEndpoint;
    private final String certificateFile;
    private final String privateKeyFile;*/
    private String defaultIndex;
    private String defaultVersion;
    private int defaultMax;
    private String defaultToken;
    private static String certificateId;
    private static IotClient client;
    private static SearchIndexRequest.Builder requestInput;

    public FleetQuery() {
        /*this.clientId = clientId;
        this.clientEndpoint = clientEndpoint;
        this.certificateFile = certificateFile;
        this.privateKeyFile = privateKeyFile;*/
        setDefaultIndex("AWS_Things");
        setDefaultMax(5);
        setDefaultToken(null);
        setDefaultVersion("2017-09-30");
    }
    public void Connect(){
        List<String> thingIds=new ArrayList<>(10);
        String model="*";
        String type="Searches";
        SearchIndexResponse response=null;
        AwsBasicCredentials awsBCreds =AwsBasicCredentials.create(
                "AKIA2BHSGK77RTX7OJ3H",
                "BRP4Iv9KhoAwoIj2DKCPWBEkR8yPjrATGKCNd3Zp"
        );
        AwsSessionCredentials awsCreds =AwsSessionCredentials.create(
                "your_access_key",
                "Your_secret_access_key",
                "aws_Session_token");
        //SampleUtil.KeyStorePasswordPair pair = SampleUtil.getKeyStorePasswordPair(this.certificateFile, this.privateKeyFile);
        //AWSIotMqttClient client = new AWSIotMqttClient(this.clientEndpoint, this.clientId, pair.keyStore, pair.keyPassword);
        client= IotClient.builder().credentialsProvider(StaticCredentialsProvider.create(awsBCreds)).region(Region.US_WEST_2).build();
        //{
        //                "indexName":"string",
        //                "maxResults":0,
        //                "nextToken":"string",
        //                "queryString":"string",
        //                "queryVersion":"string"}
        //SearchIndexRequest request;
        SearchIndexRequest.Builder request=SearchIndexRequest.builder();
        request.indexName("AWS_Things");
        request.maxResults(5);
        request.nextToken(null);
        request.queryString("attributes.SN:*");
        request.queryVersion("2017-09-30");
        response=client.searchIndex(request.build());

        for (int i = 0; i <response.things().size(); i++) {
            boolean modelFlag=false;
            boolean typeFlag=false;
            for(int j=0; j<response.things().get(i).thingGroupNames().size();j++) {
                if(response.things().get(i).thingGroupNames().get(j).equals(model)){
                    modelFlag=true;
                }
                if(response.things().get(i).thingGroupNames().get(j).equals(type)){
                    typeFlag=true;
                }
            }
            if(((typeFlag||type.equals("*"))&&(modelFlag||model.equals("*")))){
                System.out.println(response.things().get(i).thingId());
                thingIds.add(response.things().get(i).thingId());
            }
        }
        System.out.println("ThingGroups\n");
        System.out.println(response.thingGroups());
        System.out.println("Things\n");
        System.out.println(response.things());
        System.out.println("Thing Id's\n");
        System.out.println(thingIds);
        String jsonTest="{\"sn\":\"*\",\"model\":\"null\",\"type\":\"null\"}";
        System.out.println(jsonTest);
        createQuery(jsonTest,null,0,null,null,null,true);
        query();
    }

    /* Things to do ~~~~~~~
        invariants: indexName, client builder stuff, Version
        expected to be given inputs to control query:
        SN (most->least important)
        model
        type
        New args for middle man:
            query tuple(SN,Model,type), maxResults, possibly nextToken values for pagination
        new Methods needed for middle man:

            QueryMaker(SN,Model,type) SN(attribute) Model(thing group) type(thing group)
                Extra QueryMaker Logic:
                    Default AND
                    SN/model/Type if NULL use wildCard for that input

     */
    public void createQuery(String jsonSyntaxInput, String index, int results, String Token,String query, String version, boolean defaults){
        //start getting jsonStuff here
        ObjectMapper mapper= new ObjectMapper();
        String modelString;
        String typeString;
        String serialString;
        String builtQuery = null;
        try {
            QueryInputs inputsToParse=mapper.readValue(jsonSyntaxInput,QueryInputs.class);
            modelString=inputsToParse.getModel();
            typeString=inputsToParse.getType();
            serialString=inputsToParse.getSN();
            if(modelString==null){
                modelString="*";
            }
            if(typeString==null){
                typeString="*";
            }
            if(serialString==null){
                serialString="*";
            }
            builtQuery="attributes.SN:"+serialString;
            //Check if any are null and if so set to wild cards

        } catch (JsonProcessingException e) {
            e.printStackTrace();
        }
        requestInput=SearchIndexRequest.builder();
        if(defaults){
            requestInput.indexName(defaultIndex);
            requestInput.queryVersion(defaultVersion);
            requestInput.maxResults(defaultMax);
            requestInput.nextToken(defaultToken);

        }
        else {
            requestInput.indexName(index);
            requestInput.maxResults(results);
            requestInput.nextToken(Token);
            requestInput.queryVersion(version);
        }
        requestInput.queryString(builtQuery);
        System.out.println(builtQuery);
    }
    public void query(){
        System.out.println(client.searchIndex(requestInput.build()));
    }

    public String getDefaultIndex() {
        return defaultIndex;
    }
    public String getDefaultVersion(){
        return defaultVersion;
    }
    public int getDefaultMax(){
        return defaultMax;
    }
    public String getDefaultToken(){
        return defaultToken;
    }
    public void setDefaultIndex(String index){
        defaultIndex=index;
    }
    public void setDefaultMax(int max){
        defaultMax=max;
    }
    public void setDefaultToken(String token){
        defaultToken=token;
    }
    public void setDefaultVersion(String version){
        defaultVersion=version;
    }
}
